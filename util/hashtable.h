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
 extern HashTable ht;

// ---------------- Prototypes/Macros


unsigned long Hash(const element_t key, unsigned long mod);

void initHashTable();
// add to hash table
int HashTableAdd(element_t key);
// lookup
int HashTableLookUp(element_t key);
void cleanHash();


#endif // HASHTABLE_H
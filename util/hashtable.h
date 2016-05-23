/* ========================================================================== */
/* File: hashtable.h
 *
 *
 * Refactored hashtable to work with any kind of node
 *
 */
/* ========================================================================== */
#ifndef HASHTABLE_H
#define HASHTABLE_H

// ---------------- Prerequisites e.g., Requires "math.h"
#include <string.h>
#include <stdio.h>   
#include <stdint.h>
#include "common.h"                          // common functionality
#include "list.h"

// ---------------- Constants
#define MAX_HASH_SLOT 5                // number of "buckets" 10000 

// ---------------- Structures/Types

typedef int (*hashtable_compare)(element_t av, element_t bv);
typedef uint32_t (*hashtable_hash)(element_t key);

typedef struct HashTableNode {
    element_t data;                               // object hashed
    uint32_t hash;                            // hashed value to make comparison easy
    struct HashTableNode *next;              // pointer to next node
} HashTableNode;

typedef struct HashTable {
    HashTableNode *table[MAX_HASH_SLOT];     // actual hashtable
    uint32_t elementSize;
    hashtable_compare compare;
    hashtable_hash hashFn;
    freeFunction freeFn;
} HashTable;




// ---------------- Public Variables
 //extern HashTable ht;

// ---------------- Prototypes/Macros


 //Hashtable macros 
/*
int HashString(const element_t strv, int mod);


HashTable* initHashTable();
int HashTableAdd(HashTable* ht, element_t key, int (*f)(element_t, int));
int HashTableLookUp(HashTable* ht, element_t key, int (*f)(element_t, int), int (*g)(element_t, element_t));
void cleanHash();
element_t HashTableGet(HashTable* ht, element_t key, int (*f)(element_t, int), int (*g)(element_t, element_t));


// Hashtable helpers 
int cmpStrings(element_t av, element_t bv);
void concat(element_t* av, element_t bv); 
*/

void hashtable_new(HashTable* ht, int elementSize, hashtable_compare cmp,
 hashtable_hash hash, freeFunction freeFn);
void hashtable_destroy(HashTable* ht);

void hashtable_insert(HashTable* ht, element_t key, element_t data);
//int hashtable_find(HashTable* ht, element_t key);

int hashtable_get(HashTable* ht, element_t key, element_t* elem);



#endif // HASHTABLE_H
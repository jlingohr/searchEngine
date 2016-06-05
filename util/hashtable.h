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
//#include "list.h"

// ---------------- Constants
#define MAX_HASH_SLOT 997                // number of "buckets" 997 9973 10000 

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
    uint32_t size;
} HashTable;




// ---------------- Public Variables
 //extern HashTable ht;

// ---------------- Prototypes/Macros


void hashtable_new(HashTable* ht, int elementSize, hashtable_compare cmp,
 hashtable_hash hash, freeFunction freeFn);
void hashtable_destroy(HashTable* ht);

void hashtable_insert(HashTable* ht, element_t key, element_t data);

int hashtable_get(HashTable* ht, element_t key, element_t elem);
int hashtable_lookup(HashTable* ht, const element_t key);


#endif // HASHTABLE_H

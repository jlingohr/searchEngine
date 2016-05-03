/* ========================================================================== */
/* File: hashtable.h
 *
 * Project name: CS50 Tiny Search Engine
 * Component name: Crawler
 *
 * This file contains the definitions for a hashtable of urls.
 *
 */
/* ========================================================================== */
#ifndef HASHTABLE_H
#define HASHTABLE_H

// ---------------- Prerequisites e.g., Requires "math.h"
#include "common.h"                          // common functionality
#include <stdio.h>   

// ---------------- Constants
#define MAX_HASH_SLOT 10000                // number of "buckets" 10000 

// ---------------- Structures/Types

typedef struct HashTableNode {
    char *url;                               // url previously seen
    struct HashTableNode *next;              // pointer to next node
} HashTableNode;

typedef struct HashTable {
    HashTableNode *table[MAX_HASH_SLOT];     // actual hashtable
    unsigned int size;
    unsigned int n
} HashTable;

// ---------------- Public Variables
 extern HashTable URLSVisited;

// ---------------- Prototypes/Macros


unsigned long Hash(const char *str, unsigned long mod);


// add to hash table
int HashTableAdd(char * url);
// lookup
int HashTableLookUp(char * url);

#endif // HASHTABLE_H
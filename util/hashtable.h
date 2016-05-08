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
#include <string.h>
#include <stdio.h>   
#include "common.h"                          // common functionality
#include "list.h"

// ---------------- Constants
#define MAX_HASH_SLOT 10000                // number of "buckets" 10000 

// ---------------- Structures/Types



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
int HashString(const element_t strv, int mod);
int HashTableAddURL(HashTable* ht, char* url);
int HashTableLookUpURL(HashTable* ht, char* url);
void cleanHashURL(HashTable* ht);




/* Wordnode Hashtable wrappers */
int HashTableAddWord(HashTable* ht, char* word, int docID);
int HashTableLookUpWord(HashTable* ht, char* word);
int HashTableUpdateWord(HashTable* ht, char* word, int docID);
int cmpWNode(element_t wordv, element_t wNodev);
int HashWNode(const element_t wNodev, int mod);
WordNode* initWNode(char* word, int docID);
void HashTablePrintWords(HashTable* ht);
int HashTableLoadWords(HashTable* ht, char** buf);
void cleanHashWord(HashTable* ht);

/* Hashtable macros */
HashTable* initHashTable();
int HashTableAdd(HashTable* ht, element_t key, int (*f)(element_t, int));
int HashTableLookUp(HashTable* ht, element_t key, int (*f)(element_t, int), int (*g)(element_t, element_t));
void cleanHash();


/* Hashtable helpers */
int cmpStrings(element_t av, element_t bv);
void concat(element_t* av, element_t bv); 


#endif // HASHTABLE_H
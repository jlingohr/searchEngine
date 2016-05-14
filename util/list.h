
/* ========================================================================== */
/* File: list.h
 *
 * Component name: Crawler
 *
 * This file contains the definitions for a doubly-linked list of void* nodes.
 *
 */
/* ========================================================================== */
#ifndef LIST_H
#define LIST_H

// ---------------- Prerequisites e.g., Requires "math.h"
#include "common.h"                          // common functionality
#include <string.h>
 

// ---------------- Constants

// ---------------- Structures/Types

typedef struct ListNode {
    element_t* data;                           // the data for a node
    struct ListNode *next;                   // pointer to next node
} ListNode;

typedef struct List {
    ListNode *head;                          // "beginning" of the list
    ListNode *tail;                          // "end" of the list
    int len;
} List;


// ---------------- Public Variables

//extern List toVisit;

// ---------------- Prototypes/Macros



/* Generic List macros */
List* initList();
void listAdd(List* list, element_t elem);
ListNode* listRemove(List* list);
element_t listGetLast(List* list);
void listDelete(void (*f)(element_t),List* list);
int listIsEmpty(List* list);

ListNode* listGet(List* list, element_t elem, int (*f)(element_t, element_t));

void listForEach(void (*f)(element_t), List* list);
void listFoldString(void (*f) (element_t*, element_t), char** v, List* a);


void MergeSort(List* list, int len, int (*f)(element_t, element_t));
List* Merge(List* A, List* B, int (*f)(element_t, element_t));


#endif // LIST_H
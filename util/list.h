
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

typedef bool (*listIterator)(element_t);
typedef int (*list_compare)(element_t av, element_t bv);


typedef struct ListNode {
    element_t data;                           // the data for a node
    struct ListNode *next;                   // pointer to next node
} ListNode;

typedef struct List {
  ListNode *head;                          // "beginning" of the list
  ListNode *tail;                          // "end" of the list
  int length;
  int elementSize;
  list_compare compare;
  freeFunction freeFn;
} List;


// ---------------- Public Variables

//extern List toVisit;

// ---------------- Prototypes/Macros



/* Generic List macros */
/*
List* initList();
void listAdd(List* list, element_t elem);
ListNode* listRemove(List* list);
element_t listGetLast(List* list);
void listDelete(void (*f)(element_t),List* list);
int listIsEmpty(List* list);

ListNode* listGet(List* list, element_t elem, int (*f)(element_t, element_t));

void listForEach(void (*f)(element_t), List* list);
void listFoldString(void (*f) (element_t*, element_t), char** v, List* a);

int nodeCompare(element_t*, element_t elem, int (*f)(element_t, element_t));

void MergeSort(List* list, int len, int (*f)(element_t, element_t));
List* Merge(List* A, List* B, int (*f)(element_t, element_t));
*/
void list_new(List* list, int elementSize, list_compare cmp, freeFunction freeFn);
void list_destroy(List* list);

void list_prepend(List* list, element_t elem);
void list_append(List* list, element_t elem);
int list_size(List* list);

void list_for_each(List* list, listIterator iter);
void list_head(List* list, element_t elem);
void list_tail(List* list, element_t elem);

int list_dequeue(List* list, element_t elem);

int list_get(List* list, element_t key, element_t* elem);
void list_foldl(void (*f) (element_t*, element_t, element_t), element_t* out_element_p, List* list);


#endif // LIST_H

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
    int ref_count;
    struct ListNode *next, *prev;                   // pointer to next node
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
#define list_length(A) ((A)->length)
#define list_head(A) ((A)->head != NULL ? (A)->head->data : NULL)  // Mainly for testing
#define list_tail(A) ((A)->tail != NULL ? (A)->tail->data : NULL)
#define list_foreach(L, S, M, V) ListNode* _node = NULL;\
  ListNode* V = NULL;\
  for(V = _node = L->S; _node != NULL; V = _node = _node->M)

// Generic List Procedures 
void list_new(List* list, int elementSize, list_compare cmp, freeFunction freeFn);
void list_destroy(List* list);

void list_prepend(List* list, element_t elem);
void list_append(List* list, element_t elem);
int list_size(List* list);

void list_for_each(List* list, listIterator iter);

element_t list_dequeue(List* list);

int list_get(List* list, element_t key, element_t* elem);
void list_foldl(void (*f) (element_t*, element_t, element_t), element_t* out_element_p, List* list);

void list_inc_ref(ListNode* node);
void list_dec_ref(List* list, ListNode* node);


/**************
* List Algorithms
****************/
List* MergeSort(List* list, int len, int (*f)(element_t, element_t));
List* Merge(List* A, List* B, int (*f)(element_t, element_t));


#endif // LIST_H

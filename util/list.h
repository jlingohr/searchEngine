
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
 

// ---------------- Constants

// ---------------- Structures/Types

typedef struct ListNode {
    element_t data;                           // the data for a node
    struct ListNode *prev;                   // pointer to previous node
    struct ListNode *next;                   // pointer to next node
} ListNode;

typedef struct List {
    ListNode *head;                          // "beginning" of the list
    ListNode *tail;                          // "end" of the list
} List;

/* Document list nodes */

// ---------------- Public Variables

//extern List toVisit;

// ---------------- Prototypes/Macros

/* WebPage list wrappers */
WebPage* listRemovePage(List* list);
void listAddPage(List* list, WebPage* page);
void inc_ref(WebPage* page);
void dec_ref(WebPage* page);

/* DocumentNode wrappers */
void listAddDoc(List* list, DocumentNode* dNode);

/* Generic List macros */
void listAdd(List* list, element_t elem);
List* initList();
ListNode* listRemove(List* list);



#endif // LIST_H
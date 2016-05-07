
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
DocumentNode* listGetLastDNode(List* list);

/* Generic List macros */
void listAdd(List* list, element_t elem);
List* initList();
ListNode* listRemove(List* list);
element_t listGetLast(List* list);

void listForEach(void (*f)(element_t), List* list);
void listFoldString(void (*f) (element_t*, element_t), char** v, List* a);



#endif // LIST_H